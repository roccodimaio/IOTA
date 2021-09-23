// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiPlayerGameMode.h"
#include "LobbyGamemode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API ALobbyGamemode : public AMultiPlayerGameMode
{
	GENERATED_BODY()

public:
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override; 

private: 

	//VARIABLES
	uint32 NumberOfPlayers = 0;

	FTimerHandle GameStartTimer; 

	//FUNCTIONS
	void StartGame();
	
};
