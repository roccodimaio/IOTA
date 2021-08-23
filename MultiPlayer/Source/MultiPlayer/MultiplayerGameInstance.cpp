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
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());
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

void UMultiplayerGameInstance::Host()
{
	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	// Server Travel 
	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

}

void UMultiplayerGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
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
			}
		}
	}
}

void UMultiplayerGameInstance::QuitGame()
{
	APlayerController* SpecificPlayer = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), SpecificPlayer, EQuitPreference::Quit, true);
}
