// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_InGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* TimeProgressBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Minuite;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Second;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiliSeconds;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player1_TriggeredCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player1_DeathCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player2_TriggeredCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player2_DeathCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player3_TriggeredCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player3_DeathCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player4_TriggeredCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player4_DeathCount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ASAGameStateBase* gameState;

	UFUNCTION()
	void SetPlayerTD();
};
