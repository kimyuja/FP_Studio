// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/DelegateCombinations.h>
#include "W_ItemImg.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoved, UItemObject*, ItemObject);
/**
 *
 */
UCLASS()
class SNEAKYANIMALS_API UW_ItemImg : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DragDrop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UW_ItemImg> DragVisualClass;*/

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USizeBox* backgroundSizeBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* backgroundBorder;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* MyCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* itemImgSwitcher;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	class UItemObject* thisItemObject;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	FVector2D size;
	
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	float tileSize = 160.f;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void Refresh();

	FTimerHandle timerHandle;

	UFUNCTION(BlueprintCallable, Category = "Binding")
	FSlateBrush GetIconImage();

	// 델리게이트 인스턴스 선언
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRemoved OnRemoved;

	UFUNCTION(BlueprintCallable)
	void RemoveItem(class UItemObject* _ItemObject);

	// UPROPERTY()
	// class UW_CustomMap* customMapComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNewGridWidget* gridWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> newGridWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> itemSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
    TSubclassOf<UW_ItemImg> DragVisualClass;

	void SetItemObject(UItemObject* NewItemObject);

};
