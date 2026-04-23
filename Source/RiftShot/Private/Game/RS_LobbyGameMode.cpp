// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RS_LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ARS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	int32 NumberOfPlayers = GameState->PlayerArray.Num();
	
	if (NumberOfPlayers == 2)
	{
		if (UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/Arena?listen"));
		}
	}
}
