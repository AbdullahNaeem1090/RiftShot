// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RS_PlayerController.generated.h"

class ARS_Character;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class RIFTSHOT_API ARS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:	
	ARS_PlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);	
	void HandleJump(const FInputActionValue& Value);
	void HandleEquip(const FInputActionValue& Value);
	void HandleCrouch(const FInputActionValue& Value);
	
private:
	ARS_Character* GetRSCharacter();

protected:
	
	UPROPERTY()
	ARS_Character* RSCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Equip;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Crouch;

};
