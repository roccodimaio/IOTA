// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
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
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	UFUNCTION()
	virtual void LoadMainMenu() override; 
	
	UFUNCTION()
	void QuitGame(); 

	UFUNCTION()
	void RefreshServerList() override;

	void StartSession();


private:

	// VARIABLES

	TSubclassOf<class UUserWidget> MainMenuClass;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	class FString DesiredServerName; 

	// FUNCTIONS

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void CreateSession();
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);



};
