// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemComponent.h"
#include "Input/Reply.h"
#include "MapCustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UMapCustomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override; 
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* customUIBgm;

    UAudioComponent* audioComponent;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UNewGridWidget* NewGridWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* BackgroundImgSwitcher;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* floorTopViewSwitcher;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* Btn1WidgetSwitcher;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* Btn2WidgetSwitcher;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* Btn3WidgetSwitcher;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* Btn4WidgetSwitcher;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	float tileSize = 80.f;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* BackgroundBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* GActorBtn1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* GActorBtn2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* GActorBtn3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* GActorBtn4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* min;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* sec;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* timerProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UGimmickSelectionWidget* gimmickSelectionWidget;

public:
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UNewGridWidget> gWidget;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* DraggedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
	class ATestPlayer* myPlayer;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> GimmickSelectionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMouseInsideWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMouseInsideButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 playerRandNum;


	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void UpdateTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void TimerFinished();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	FText GetFormattedMinutes();
	UFUNCTION(BlueprintCallable, Category = "Timer")
	FText GetFormattedSeconds();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetProgressBarPercent();

	FTimerHandle CountdownTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 timeRemaining = 60;

	UFUNCTION()
    void OnGActorBtn1Clicked();

    UFUNCTION()
    void OnGActorBtn2Clicked();

    UFUNCTION()
    void OnGActorBtn3Clicked();

    UFUNCTION()
    void OnGActorBtn4Clicked();

	void HandleButtonClicked(UButton* _ClickedButton, FVector2D _GSWPos);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D gswPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCanvasPanelSlot* canvasSlot;

    UFUNCTION()
    void SetGSWBasicPos();

    UFUNCTION()
    void RemovedItemCheck(class UItemObject* _itemObject);
};
