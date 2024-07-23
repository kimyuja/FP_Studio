// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemComponent.h"
#include "W_ItemSlot.h"
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

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UW_CustomMap* CustomMapGridWidget;*/

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UNewGridWidget* NewGridWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	float tileSize = 160.f;

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



public:

	//UFUNCTION(BlueprintCallable, Category = "UI")
	//void InitializeItemSlots();

	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	//TArray<UW_ItemSlot*> itemSlotWidgets;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> itemSlotWidgetClass;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UW_CustomMap> gWidget;*/
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UNewGridWidget> gWidget;

	// UW_ItemSlot* itemSlotWidget;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* DraggedItem;

	FReply OnBackgroundBorderMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void BindButtonEvent();
	
	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
    void OnButtonUnhovered();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> GimmickSelectionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UGimmickSelectionWidget* gimmickSelectionWidget;
	
	UFUNCTION()
	void OnSelectionWidgetMouseLeave(const FPointerEvent& InMouseEvent);UFUNCTION()
    void OnSelectionWidgetMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMouseInsideWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMouseInsideButton;

};
