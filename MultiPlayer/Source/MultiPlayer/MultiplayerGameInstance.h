// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "MultiplayerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMultiplayerGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public: 
	UMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer);

	// Functions
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(Exec) // Allows function to be access from Console 
	void Host() override;

	UFUNCTION(Exec)
	void Join(const FString& Address) override;

	UFUNCTION()
	virtual void LoadMainMenu() override; 
	
	UFUNCTION()
	void QuitGame(); 


private:

	TSubclassOf<class UUserWidget> MainMenuClass;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;



};
