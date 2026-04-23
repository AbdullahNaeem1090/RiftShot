// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RS_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RIFTSHOT_API ARS_LobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
