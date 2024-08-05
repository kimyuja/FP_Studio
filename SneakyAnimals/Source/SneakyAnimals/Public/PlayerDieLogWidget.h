// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDieLogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UPlayerDieLogWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DieLogText;
	
	UPROPERTY(EditAnywhere)
	FText playerName;


	
	UFUNCTION(BlueprintCallable)
    void ShowText(const FText& NewText);
	
	UFUNCTION(BlueprintCallable)
    void GetPlayerName(AActor* ActivePlayer);

};
