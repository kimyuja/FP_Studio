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

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GimmickBtn1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GimmickBtn2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ClearBtn;

	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* GmTxt1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* GmTxt2;


	UPROPERTY()
	FOnCustomMouseLeaveEvent OnCustomMouseLeave;
	UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnCustomMouseEnterEvent OnCustomMouseEnter;


	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> newGridWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	class UNewGridWidget* gridWidget;*/


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

	UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateClearBtnState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bUsedClearGimmick;
	
	// 마녀의집
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WitchsHouse")
	bool bBookShelfInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WitchsHouse")
	bool bBroomstickInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WitchsHouse")
	bool bPotionInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WitchsHouse")
	bool bPotInWorld = false;

	// 잠수함
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	bool bComputerInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	bool bPeriscopeInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	bool bPressBtnInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	bool bWhistleInWorld = false;

	// 슈퍼마켓
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Supermarket")
	bool bBottleInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Supermarket")
	bool bCartInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Supermarket")
	bool bCleanerInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Supermarket")
	bool bShowcaseInWorld = false;
	
	// 금고
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bank")
	bool bGoldbarInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bank")
	bool bHandleInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bank")
	bool bLaserInWorld = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bank")
	bool bSwitchInWorld = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
	class ATestPlayer* myPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
	class UItemComponent* itemComp;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UItemObject* itemObject;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetItemObejct(class UItemObject* _ItemObj);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 playerRandNum;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetPlayerRandNum(int _PlayerNum);

	UFUNCTION(BlueprintCallable, Category = "DragDrop")
	void BindItemObjByBtn(TSubclassOf<AGimmick> GimmickClass, int32 _ActiveType);

	// 마녀의집
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WitchsHouse")
	TArray<class AWH_BookshelfGimmick*> bookShelfActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WitchsHouse")
	TArray<class AWH_BroomstickGimmick*> broomStickActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WitchsHouse")
	TArray<class AWH_PotionGimmick*> potionActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WitchsHouse")
	TArray<class AWH_WitchCauldronGimmick*> cauldronActorArr;

	// 잠수함
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ASM_ComputerGimmick*> computerActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ASM_PeriscopeGimmick*> periscopeActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ASM_PressButtonGimmick*> pressbuttonActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ASM_WhistleGimmick*> whistleGimmickActorArr;
	
	// 슈퍼마켓
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ASP_BottleGimmick*> bottleActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ASP_CartGimmick*> cartActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ASP_CleanerGimmick*> cleanerActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ASP_ShowcaseGimmick*> showcaseActorArr;

	// 금고
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ABS_GoldBarGimmick*> goldbarActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ABS_HandleGimmick*> handleActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ABS_LaserGimmick*> laserActorArr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Submarine")
	TArray<class ABS_SwitchGimmick*> switchActorArr;
};
