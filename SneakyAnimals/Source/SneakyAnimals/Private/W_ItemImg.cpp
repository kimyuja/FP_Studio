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
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/DragDropOperation.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
// #include "DragWidget.h" 

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

	itemObject = NewObject<UItemObject>(this);

	if (!itemObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("itemobject is nullptr in itemImg code"));
	}

	/*UW_ItemSlot* itemSlotW = CreateWidget<UW_ItemSlot>(GetWorld(), itemSlotWidget);
	itemObject = itemSlotW->itemObject;*/

}

void UW_ItemImg::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (backgroundBorder)
	{	// ���콺�� ������ �� ���� ���� ����
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

void UW_ItemImg::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragDropOperation* dragDropOperation = NewObject<UDragDropOperation>(this, UDragDropOperation::StaticClass());

	if (dragDropOperation)
	{
		dragDropOperation->Payload = itemObject;
		dragDropOperation->DefaultDragVisual = this;  // �巡�� �� ���� ���� ����
		dragDropOperation->Pivot = EDragPivot::CenterCenter;

		// �巡�׸� �����ϸ� �κ��丮���� ���ŵ� ������ �����ϰ� remove

		/*dragDropOperation->OnDragCancelled.AddDynamic(this, &UW_ItemImg::OnDragCancelled);
		dragDropOperation->OnDrop.AddDynamic(this, &UW_ItemImg::OnDragDrop);*/

		OutOperation = dragDropOperation;

		// ���� ������ �θ𿡼� ����
		// RemoveFromParent();
	}

}

// ���콺 ������ �巡�� ����
FReply UW_ItemImg::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// ���� ���콺 ������ �� �巡�� �̺�Ʈ ����
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

// �κ��丮�� �ٽ� �׸� ������ ȣ��
void UW_ItemImg::Refresh()
{
	
	if (itemObject != nullptr)
	{
		FIntPoint itemDimensions = itemObject->GetDimensions();
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


}

FSlateBrush UW_ItemImg::GetIconImage()
{
	FSlateBrush brush;

	if (itemObject)
	{
		UMaterialInterface* iconMaterial = itemObject->GetIcon();

		if (iconMaterial)
		{
			// MakeBrushfromMaterail
			brush.SetResourceObject(iconMaterial);	

			brush.ImageSize = FVector2D(FMath::TruncToFloat(size.X), FMath::TruncToFloat(size.Y));
		}
	}

	return brush;
}

void UW_ItemImg::RemoveItem(UItemObject* ItemObject)
{
	// ��������Ʈ ȣ�� - �Ķ���� ����
	OnRemoved.Broadcast(ItemObject);
	RemoveFromParent();
}

//void UW_ItemImg::OnDragCancelled(UDragDropOperation* Operation)
//{
//	if (Operation && Operation->Payload == itemObject)
//	{
//		RemoveItem(itemObject);
//	}
//}
//
//void UW_ItemImg::OnDragDrop(UDragDropOperation* Operation)
//{
//	if (Operation && Operation->Payload == itemObject)
//	{
//		RemoveItem(itemObject);
//	}
//}

