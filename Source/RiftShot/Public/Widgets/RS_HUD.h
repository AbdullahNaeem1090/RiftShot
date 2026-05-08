// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RS_HUD.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsCenter;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsLeft;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsRight;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsTop;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsBottom;
	
	UPROPERTY()
	float CrosshairSpread;
	
	UPROPERTY()
	FLinearColor CrosshairColor;
};

UCLASS()
class RIFTSHOT_API ARS_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;
	
private:
	FHUDPackage HUDPackage;
	
	void DrawCrosshair(UTexture2D* Texture, FVector2D Spread, FVector2D ViewportCenter,FLinearColor Color);
	
	float SpreadMagnitude=10.f;
	
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
