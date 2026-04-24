
#include "Player/RS_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Characters/RS_Character.h"
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
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(IA_Move,ETriggerEvent::Triggered,this,&ARS_PlayerController::HandleMove);
	EnhancedInputComponent->BindAction(IA_Look,ETriggerEvent::Triggered,this,&ARS_PlayerController::HandleLook);
	EnhancedInputComponent->BindAction(IA_Jump,ETriggerEvent::Started,this,&ARS_PlayerController::HandleJump);
	EnhancedInputComponent->BindAction(IA_Equip,ETriggerEvent::Started,this,&ARS_PlayerController::HandleEquip);
	EnhancedInputComponent->BindAction(IA_Crouch,ETriggerEvent::Started,this,&ARS_PlayerController::HandleCrouch);
	EnhancedInputComponent->BindAction(IA_Aim,ETriggerEvent::Started,this,&ARS_PlayerController::HandleAimPress);
	EnhancedInputComponent->BindAction(IA_Aim,ETriggerEvent::Completed,this,&ARS_PlayerController::HandleAimReleased);
}

ARS_Character* ARS_PlayerController::GetRSCharacter()
{
	if (!IsValid(RSCharacter))
	{
		RSCharacter=Cast<ARS_Character>(GetCharacter());
	}
	return RSCharacter;
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

void ARS_PlayerController::HandleEquip(const FInputActionValue& Value)
{
	if (GetRSCharacter()) RSCharacter->EquipPressed();
}

void ARS_PlayerController::HandleCrouch(const FInputActionValue& Value)
{
	if (GetRSCharacter()) RSCharacter->ToggleCrouch();
}

void ARS_PlayerController::HandleAimPress(const FInputActionValue& Value)
{
	if (GetRSCharacter()) RSCharacter->StartAiming();
}

void ARS_PlayerController::HandleAimReleased(const FInputActionValue& Value)
{
	if (GetRSCharacter()) RSCharacter->StopAiming();
}


