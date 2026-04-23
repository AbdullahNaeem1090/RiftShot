// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

class UTextBlock;

UCLASS()
class RIFTSHOT_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetDisplayText(const FString& Text);
	
	UFUNCTION(BlueprintCallable)
	void ShowDisplayText(APawn* InPawn);
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DisplayText;

	
};
