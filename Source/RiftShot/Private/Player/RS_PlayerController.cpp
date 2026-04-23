// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RS_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"

ARS_PlayerController::ARS_PlayerController()
{
	bReplicates = true;
}

void ARS_PlayerController::BeginPlay()
{
	check(DefaultMappingContext);
	
	Super::BeginPlay();
	
	if (!IsLocalController()) return;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultMappingContext,0);
	}
	
}

void ARS_PlayerController::SetupInputComponent()
{
	check(IA_Move);
	check(IA_Look);
	check(IA_Jump);
	
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(IA_Move,ETriggerEvent::Triggered,this,&ARS_PlayerController::HandleMove);
	EnhancedInputComponent->BindAction(IA_Look,ETriggerEvent::Triggered,this,&ARS_PlayerController::HandleLook);
	EnhancedInputComponent->BindAction(IA_Jump,ETriggerEvent::Started,this,&ARS_PlayerController::HandleJump);
	
}

void ARS_PlayerController::HandleMove(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;
	
	const FVector2D InputValue= Value.Get<FVector2D>();
	const FRotator ControllerYawRotation(0.f,GetControlRotation().Yaw,0.f);
	const FVector RightVector=FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::X);
	const FVector ForwardVector=FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::Y);
	
	ControlledPawn->AddMovementInput(ForwardVector,InputValue.Y);
	ControlledPawn->AddMovementInput(RightVector,InputValue.X);
	
}

void ARS_PlayerController::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void ARS_PlayerController::HandleJump(const FInputActionValue& Value)
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (!ControlledCharacter) return;
	ControlledCharacter->Jump();
}
