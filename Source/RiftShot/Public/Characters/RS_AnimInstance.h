#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RS_AnimInstance.generated.h"

class ARS_Character;

UCLASS()
class RIFTSHOT_API URS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<ARS_Character> Character;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement", meta=(AllowPrivateAccess=true))
	bool bIsAccelerating;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement", meta=(AllowPrivateAccess=true))
	bool bIsInAir;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement", meta=(AllowPrivateAccess=true))
	float Speed;
};
