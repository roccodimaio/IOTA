// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiplayerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMultiplayerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public: 
	UMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(Exec) // Allows function to be access from Console 
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& Address);
	

};
