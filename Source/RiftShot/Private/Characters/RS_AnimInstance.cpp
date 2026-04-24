// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RS_AnimInstance.h"

#include "Characters/RS_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void URS_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character=Cast<ARS_Character>(TryGetPawnOwner());
	
}

void URS_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!IsValid(Character))
		Character=Cast<ARS_Character>(TryGetPawnOwner());
	
	if (!IsValid(Character)) return;
	
	FVector Velocity=Character->GetVelocity();
	Velocity.Z=0;
	Speed= Velocity.Size();
	
	bIsInAir=Character->GetCharacterMovement()->IsFalling();
	
	bIsAccelerating=Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
	
	bIsWeaponEquipped=Character->IsWeaponEquipped();
	
	bIsCrouched=Character->bIsCrouched;
	
	bIsAiming=Character->IsAiming();
}
