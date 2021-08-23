// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelInGameMenu_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitInGameMenu_Button;
protected:

	// Functions
	virtual bool Initialize();

private:

	UFUNCTION()
	void QuitInGameMenu();

	UFUNCTION()
	void CancelInGameMenu();
	
};
