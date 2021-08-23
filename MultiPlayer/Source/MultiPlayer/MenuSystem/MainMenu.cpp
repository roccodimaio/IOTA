// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"



bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if(!Success) return false;

	if (!ensure(Host_Button != nullptr)) return false;

	Host_Button->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(Join_Button != nullptr)) return false;
	Join_Button->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinMenu_Button != nullptr)) return false;
	CancelJoinMenu_Button->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinByIP_Button != nullptr)) return false;
	JoinByIP_Button->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(ExitMainMenu_Button != nullptr)) return false; 
	ExitMainMenu_Button->OnClicked.AddDynamic(this, &UMainMenu::ExitMainMenu);

	return true; 
}

/*
void UMainMenu::SetMenuInterface(IMenuInterface* SetMenuInterface)
{
	this->MenuInterface = SetMenuInterface;
}
*/

/*
void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return; 

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}
*/

/*
void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}
*/

/*
void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
*/

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return; 
	if (!ensure(JoinMenu != nullptr)) return; 

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ExitMainMenu()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->QuitGame();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		if (!ensure(IPAddressField != nullptr)) return;

		const FString& IPAddress = IPAddressField->GetText().ToString();
		MenuInterface->Join(IPAddress);
	}
	
}
