// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MultiPlayerGameMode.h"
#include "OnlineSubsystem.h"

#include "Interfaces/OnlineSessionInterface.h"

const static FName SESSION_NAME = TEXT("MySessionGame");
const static FName Server_NAME_SETTINGS_KEY = TEXT("ServerName");


UMultiplayerGameInstance::UMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/_MenuSystem/MainMenu/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr)) return;

	MainMenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/_MenuSystem/InGameMenu/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;

}

void UMultiplayerGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		
		SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnJoinSessionComplete);

			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem!"));
	}
	//UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());
}

void UMultiplayerGameInstance::LoadMenu()
{
	if (!ensure(MainMenuClass != nullptr)) return; 

	Menu = CreateWidget<UMainMenu>(this, MainMenuClass);

	if (!ensure(Menu != nullptr)) return; 

	Menu->bIsFocusable = true;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UMultiplayerGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);

	if (!ensure(InGameMenu != nullptr)) return;

	InGameMenu->bIsFocusable = true;

	InGameMenu->Setup();

	InGameMenu->SetMenuInterface(this);
}

void UMultiplayerGameInstance::Host(FString ServerName)
{
	UE_LOG(LogTemp, Warning, TEXT("In MultiplayerGameInstance->Host"));
	
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("In MultiplayerGameInstance->Host->SessionInterface.IsValid"));
		
		// Get name of an existing session
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		// Delete existing session if one exist (Can only host one session at a time) 
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else // If no existing session exist then create one
		{
			CreateSession();
		}
	}
}

void UMultiplayerGameInstance::Join(uint32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("In MultiplayerGameInstance.cpp -> Join"));

	if (!SessionInterface.IsValid()) return; 

	if (!SessionSearch.IsValid()) return; 

	if (Menu != nullptr)
	{
		//Menu->SetServerList({ "Test1", "Test2" });

	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

}

void UMultiplayerGameInstance::LoadMainMenu()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) return;

	//PlayerController->ClientTravel("/Game/Content/_Levels/MainMenu/MainMenu.umap", ETravelType::TRAVEL_Absolute);

	UWorld* World = GetWorld();
	if (World)
	{
		if (World->IsServer())
		{
			AMultiPlayerGameMode* GM = World->GetAuthGameMode<AMultiPlayerGameMode>();
			if (GM)
			{
				GM->ReturnToMainMenuHost();
			}
		}
		else
		{
			APlayerController* PC = GetFirstLocalPlayerController();
			if (PC)
			{
				PC->ClientReturnToMainMenu("Back to main menu");
				//PC->ClientReturnToMainMenuWithTextReason(FText& ReturnReason);
			}
		}
	}
}

void UMultiplayerGameInstance::QuitGame()
{
	APlayerController* SpecificPlayer = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), SpecificPlayer, EQuitPreference::Quit, true);
}

void UMultiplayerGameInstance::CreateSession()
{
	// Create a new session
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		
		// If not using Steam (mostly for testing)
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
				
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true; 
		SessionSettings.Set(Server_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);  // Set Server / Session Name

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}
void UMultiplayerGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;

		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UMultiplayerGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create sesson!"));
		return;
	}

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	// Server Travel 
	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UMultiplayerGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	// Once existing session is successfully deleted create a new session
	if (Success)
	{
		CreateSession();
	}
}

void UMultiplayerGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Completed Find Session"));
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		TArray<FServerData> ServerData;

		// Populate server list for testing purposes
		/*ServerNames.Add("Test Server 1");
		ServerNames.Add("Test Server 2");
		ServerNames.Add("Test Server 3");*/

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session Named: %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections; // Number of max current connections available 
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections; // Number of max connects available - Number of connections available
			Data.HostUserName = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(Server_NAME_SETTINGS_KEY, ServerName))
			{
				Data.ServerName = ServerName;
			}
			else
			{
				Data.ServerName = "Could not find server name!";
			}

			ServerData.Add(Data); 
		}
		
		Menu->SetServerList(ServerData); 
	}
}



void UMultiplayerGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	
	if (!SessionInterface.IsValid()) return;

	FString Address; 

	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cound not get ConnectString"));
		return; 
	}

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	
}
