// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GimmickSelectionWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCustomMouseLeaveEvent, const FPointerEvent&, InMouseEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomMouseEnterEvent, const FGeometry&, MyGeometry, const FPointerEvent&, InMouseEvent);


UCLASS()
class SNEAKYANIMALS_API UGimmickSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


public:

	

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GimmickBtn1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GimmickBtn2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ClearBtn;

	UPROPERTY()
	FOnCustomMouseLeaveEvent OnCustomMouseLeave;
	UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnCustomMouseEnterEvent OnCustomMouseEnter;




	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> newGridWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	class UNewGridWidget* gridWidget;



	UFUNCTION()
	void OnGimmickBtn1Clicked();
	UFUNCTION()
	void OnGimmickBtn2Clicked();
	UFUNCTION()
	void OnClearBtnClicked();


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 itemType;

	UFUNCTION(BlueprintCallable)
	void BindBtnWithActiveType(FName _BtnName);

	UPROPERTY()
	FName buttonName;

	/*UPROPERTY(BlueprintReadWrite, Category = "Settings")
    bool bUsedClearGimmick = false;*/

	 UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateClearBtnState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bUsedClearGimmick;


};
