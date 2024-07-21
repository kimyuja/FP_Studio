// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ItemSlot.generated.h"

/**
 *
 */
UCLASS()
class SNEAKYANIMALS_API UW_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;


public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UButton* itemBtn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UImage* itemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UTextBlock* itemCost;

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMapCustomWidget* mapCustomWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> newGridWidget;

	UFUNCTION(BlueprintCallable)
	void OnItemBtnClicked();

	UFUNCTION(BlueprintCallable)
	void SetMapCustomWidget(UMapCustomWidget* Widget);

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
    class UItemComponent* itemComponent;
	
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
    class UItemObject* itemObject;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void GimmickActorSetLoc();

	const UWorld* World;

	// 버튼에 연결할 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWH_BookshelfGimmick> ShelfG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWH_BroomstickGimmick> BroomG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWH_PotionGimmick> PotionG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWH_WitchCauldronGimmick> potG;
	// 

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetItemIcon(const FString& TexturePath);

	/*UPROPERTY(EditAnywhere)
	class UW_CustomMap* gridWidget;*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UI")
	class UNewGridWidget* gridWidget;

	UPROPERTY(EditAnywhere)
	int32 itemType;

	UFUNCTION(BlueprintCallable)
	void InitializeItemSlot(int32 _ItemType);

	UFUNCTION(BlueprintCallable)
	void SetCurrentCost();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GimmickSelectionWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	class UGimmickSelectionWidget* gimmickSelectionWidget;

	UFUNCTION()
	void OnMouseEnterWidget();

	UFUNCTION()
	void OnMouseLeaveWidget();

	UFUNCTION()
	void OnSelectionWidgetMouseLeave(const FPointerEvent& InMouseEvent);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
};
