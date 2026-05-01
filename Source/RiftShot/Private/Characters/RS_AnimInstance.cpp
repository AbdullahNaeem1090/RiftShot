
#include "Characters/RS_AnimInstance.h"
#include "Characters/RS_Character.h"
#include "UObject/ObjectMacros.h"

// Initialize 
void URS_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character=Cast<ARS_Character>(TryGetPawnOwner());
}

// Update
void URS_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!Character)
		Character=Cast<ARS_Character>(TryGetPawnOwner());
	
	if (!Character) return;
	
	Pitch = Character->IsLocallyControlled() ? Character->GetBaseAimRotation().Pitch : Character->AimPitch ;
	bIsAiming = Character->IsAiming();
	DistanceToGround=Character->DistanceToGround;
}

// Interface
void URS_AnimInstance::UpdateGait_Implementation(ERS_Gait NewGait)
{
	CurrentGait=NewGait;
}