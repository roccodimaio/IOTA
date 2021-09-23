// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGamemode.h"
#include "TimerManager.h"
#include "MultiplayerGameInstance.h"

void ALobbyGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	if (NumberOfPlayers >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached 2 players"));
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGamemode::StartGame, 10.0f);
		
		
	}
}

void ALobbyGamemode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGamemode::StartGame()
{
	auto GameInstance = Cast<UMultiplayerGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;

	GameInstance->StartSession(); // Starts the session so no new members can join 

	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/_Levels/MainLevel/GameMap?listen");
}
