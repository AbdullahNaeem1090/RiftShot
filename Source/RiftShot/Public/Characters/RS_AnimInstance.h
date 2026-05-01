#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interfaces/RS_AnimInterface.h"
#include "RS_Character.h"
#include "RS_AnimInstance.generated.h"

// Forward declaration (optional since already included, but fine practice)
class ARS_Character;

UCLASS()
class RIFTSHOT_API URS_AnimInstance 
	: public UAnimInstance
	, public IRS_AnimInterface
{
	GENERATED_BODY()

public:

	/** Interface Implementation */
	virtual void UpdateGait_Implementation(ERS_Gait NewGait) override;

protected:

	/** UAnimInstance Overrides */
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	/** Cached Character Reference */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ARS_Character> Character;

	/** Aim Data */
	UPROPERTY(BlueprintReadOnly, Category = "Animation|Aim")
	float Pitch = 0.f;

	/** Ground Detection */
	UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
	float DistanceToGround = 0.f;

	/** State Flags */
	UPROPERTY(BlueprintReadOnly, Category = "Animation|State")
	bool bIsAiming = false;

	/** Movement State */
	UPROPERTY(BlueprintReadOnly, Category = "Animation|State")
	ERS_Gait CurrentGait = ERS_Gait::Jog;
};