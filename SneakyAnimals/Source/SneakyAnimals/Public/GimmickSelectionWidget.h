// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GimmickSelectionWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseLeaveEvent, const FPointerEvent&, InMouseEvent);

UCLASS()
class SNEAKYANIMALS_API UGimmickSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GimmickBtn1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GimmickBtn2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ClearBtn;

	UPROPERTY()
	FOnMouseLeaveEvent OnCustomMouseLeave;


	UFUNCTION()
	void OnGimmickBtn1Clicked();
	UFUNCTION()
	void OnGimmickBtn2Clicked();
	UFUNCTION()
	void OnClearBtnClicked();



};
