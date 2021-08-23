// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(CancelInGameMenu_Button != nullptr)) return false;
	CancelInGameMenu_Button->OnClicked.AddDynamic(this, &UInGameMenu::CancelInGameMenu);

	if (!ensure(QuitInGameMenu_Button != nullptr)) return false;
	QuitInGameMenu_Button->OnClicked.AddDynamic(this, &UInGameMenu::QuitInGameMenu);

	return true;
}

void UInGameMenu::QuitInGameMenu()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}

void UInGameMenu::CancelInGameMenu()
{
	if (MenuInterface != nullptr)
	{
		this->Teardown();
	}
}
