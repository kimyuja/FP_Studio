// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "NewGridWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UNewGridWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override; 
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override; 
	virtual bool NativeOnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* gridBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* gridCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    class UW_ItemImg* itemImgWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UW_ItemImg> itemImgWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UCanvasPanelSlot* canvasSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UItemComponent* itemComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UMapCustomWidget* mapCustomWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> itemSlotWidget;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<class ULineStructure*> lines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float tileSize = 160.f;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UItemObject* itemObject;*/

	FTimerHandle SetGridSizeTimerHandle;
	FTimerHandle DrawGridLineTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
	FIntPoint draggedItemTopLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
	bool bDrawDropLoc;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
	bool bRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
	bool bDown;


public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void GridBorderSetSize(float _TileSize);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void Refresh();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void CreateLineSegments();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CreateVerticalLine();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void CreateHorizantalLine();

	// 델리게이트 바인딩 할 함수
	UFUNCTION(BlueprintCallable)
	void OnItemRemoved(class UItemObject* _ItemObject);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void DrawGridLine();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	FVector2D GetGridBorderTopLeft() const;

	FReply OnGridBorderMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION(BlueprintCallable, Category = "DragDrop")
	class UItemObject* GetPayload(UDragDropOperation* _DragDropOperation) const;
	
	UFUNCTION(BlueprintCallable, Category = "DragDrop")
	bool IsRoomAvailableForPayload(class UItemObject* _Payload) const;
	
	UFUNCTION(BlueprintCallable, Category = "DragDrop")
	void MousePositionInTile(FVector2D _MousePos);

	UFUNCTION(BlueprintCallable, Category = "DragDrop")
	void CallIncreseCostFunc(class UMapCustomWidget* _MapCustomWid, class  UItemObject* _ItemObj);
	
	UPROPERTY(BlueprintReadWrite, Category = "MakeBox")
	FSlateBrush boxBrush;

	UFUNCTION(BlueprintCallable, Category = "DragDrop")
	void FindItemClass(class UItemObject* _ItemObj);

};
