// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ItemImg.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SizeBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include "timerManager.h"
#include "ItemObject.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanelSlot.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <W_ItemSlot.h>
#include "NewGridWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>
#include "MyDragDropOperation.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/DragDropOperation.h>

bool UW_ItemImg::Initialize()
{
	Super::Initialize();

	return false;
}

void UW_ItemImg::NativeConstruct()
{
	Super::NativeOnInitialized();

	if (backgroundSizeBox && backgroundBorder)
	{
		backgroundSizeBox->SetContent(backgroundBorder);
	}

	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UW_ItemImg::Refresh, 0.2f, false);

	UW_ItemSlot* itemSlotW = CreateWidget<UW_ItemSlot>(GetWorld(), itemSlotWidget);
	// itemObject = itemSlotW->itemObject;

}

void UW_ItemImg::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (backgroundBorder)
	{	
		// ���콺�� ������ �� ���� ���� ����
		backgroundBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.2f));
	}
}

void UW_ItemImg::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (backgroundBorder)
	{
		// ���콺�� ������ �� ���� ������ ����
		backgroundBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
	}
}

// �巡�׽� �߻��ϴ� �̺�Ʈ ó��
void UW_ItemImg::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UW_ItemImg* DragVisual = CreateWidget<UW_ItemImg>(GetWorld()->GetFirstPlayerController(), DragVisualClass);
	
	if (DragVisual)
	{
		DragVisual->SetItemObject(this->thisItemObject);
		// DragVisual->SetDesiredSizeInViewport(FVector2D(StaticItemObject->dimensions.X * tileSize, StaticItemObject->dimensions.Y * tileSize));
		UCanvasPanel* rootCanvas = Cast<UCanvasPanel>(MyCanvas);

		// UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DragVisual->Slot);
		UCanvasPanelSlot* CanvasSlot = rootCanvas->AddChildToCanvas(DragVisual);

		if (CanvasSlot)
		{
			CanvasSlot->SetSize(FVector2D(thisItemObject->dimensions.X * tileSize, thisItemObject->dimensions.Y * tileSize));

			UE_LOG(LogTemp, Warning, TEXT("!!! staticItemObject size : (%f, %f)"), thisItemObject->dimensions.X * tileSize, thisItemObject->dimensions.Y * tileSize);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("bbb"));
		}

		// �� ��� ������ ���
		UMyDragDropOperation* dragDropOperation = NewObject<UMyDragDropOperation>(this);
		// UMyDragDropOperation* dragDropOperation = Cast<UMyDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UMyDragDropOperation::StaticClass()));

		if (dragDropOperation)
		{
			dragDropOperation->Payload = thisItemObject;
			dragDropOperation->DefaultDragVisual = rootCanvas;  // �巡�� �� ���� ���� ����
			// dragDropOperation->draggedItemObj->dimensions
			dragDropOperation->Pivot = EDragPivot::CenterCenter;

			/*this->size.X = thisItemObject->dimensions.X * tileSize;
			this->size.Y = thisItemObject->dimensions.Y * tileSize;*/

			dragDropOperation->draggedItemObj = thisItemObject;

			UE_LOG(LogTemp, Warning, TEXT("My size : (%f, %f)"), this->size.X, this->size.Y);
			//UE_LOG(LogTemp, Warning, TEXT("this itemObject dimension : (%d, %d)"), DragVisual->thisItemObject->dimensions.X, DragVisual->thisItemObject->dimensions.Y);

			// �巡�׸� �����ϸ� �κ��丮���� ���ŵ� ������ �����ϰ� remove
			RemoveItem(thisItemObject);
			RemoveFromParent();

			OutOperation = dragDropOperation;
		}
	}
}

// ���콺 ������ �巡�� ����
FReply UW_ItemImg::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return FReply::Unhandled();
}

// �κ��丮�� �ٽ� �׸� ������ ȣ��
void UW_ItemImg::Refresh()
{
	/*
	if (StaticItemObject != nullptr)
	{
		FIntPoint itemDimensions = StaticItemObject->GetDimensions();
		size.X = itemDimensions.X * tileSize;
		size.Y = itemDimensions.Y * tileSize;

		backgroundSizeBox->SetWidthOverride(size.X);
		backgroundSizeBox->SetHeightOverride(size.Y);

		UE_LOG(LogTemp, Warning, TEXT("GetDimensions XY in itemImg %f %f"), size.X, size.Y);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemObject is nullptr at ItemImgWidget"));
	}

	if (itemImage)
	{
		// UCanvasPanelSlot* itemImgToCanvasSlot = Cast<UCanvasPanelSlot>(itemImage->Slot);

		UCanvasPanelSlot* itemImgToCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(itemImage);

		if (itemImgToCanvasSlot)
		{
			itemImgToCanvasSlot->SetSize(size);

			UE_LOG(LogTemp, Warning, TEXT("ICON IMG SIZE is %f %f"), size.X, size.Y);
		}

		itemImage->SetBrush(GetIconImage());
	}

	*/
}


FSlateBrush UW_ItemImg::GetIconImage()
{
	FSlateBrush brush;

	if (thisItemObject)
	{
		UMaterialInterface* iconMaterial = thisItemObject->GetIcon();

		if (iconMaterial)
		{
			// MakeBrushfromMaterail
			brush.SetResourceObject(iconMaterial);	

			brush.ImageSize = FVector2D(FMath::TruncToFloat(size.X), FMath::TruncToFloat(size.Y));
		}
	}

	return brush;
}

void UW_ItemImg::RemoveItem(UItemObject* _ItemObject)
{
	// ��������Ʈ ȣ�� - �Ķ���� ����
	OnRemoved.Broadcast(_ItemObject);
}

void UW_ItemImg::SetItemObject(UItemObject* NewItemObject)
{
	thisItemObject = NewItemObject;
}


