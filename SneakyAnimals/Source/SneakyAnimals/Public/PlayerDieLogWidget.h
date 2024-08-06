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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DieLogText;
	
	UPROPERTY(EditAnywhere)
	FText playerName;

	UPROPERTY(EditAnywhere)
	FText txt;

	FTimerHandle DelayTimerHandle;

	
	UFUNCTION(BlueprintCallable)
    void ShowText(const FText& NewText);
	
	UFUNCTION(BlueprintCallable)
    void GetLogSet(FText _ActivePlayer, class AGimmick* _GimmickActor, int32 _ActiveType);

};
