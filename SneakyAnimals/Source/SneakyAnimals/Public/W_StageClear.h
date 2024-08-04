// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_StageClear.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_StageClear : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DeathCount_P1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DeathCount_P2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DeathCount_P3;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DeathCount_P4;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ClearTime;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* NextRoundTime;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LevelNum_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LevelName_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player1_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player2_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player3_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player4_Text;
	
	UPROPERTY(EditAnywhere, Transient ,meta = (BindWidgetAnim))
	class UWidgetAnimation* GameEnd;

	UPROPERTY()
	class ASAGameStateBase* gameState;

	float curtime;

	bool bIsClear = false;

	bool bIsSuccess = false;

	UFUNCTION(BlueprintCallable)
	void SetWidgetState();
};
