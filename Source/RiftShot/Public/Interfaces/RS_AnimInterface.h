// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RS_AnimInterface.generated.h"

enum class ERS_Gait : uint8;
// This class does not need to be modified.
UINTERFACE(BlueprintType)
class URS_AnimInterface : public UInterface
{
	GENERATED_BODY()
};


class RIFTSHOT_API IRS_AnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Gameplay")
	void UpdateGait(ERS_Gait NewGait);
	//
	// // Return value example
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Gameplay")
	// bool IsArmed() const;
};
