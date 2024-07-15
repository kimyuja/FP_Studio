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
	{	// 마우스가 들어왔을 때 보더 색상 변경
		backgroundBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.2f));
	}
}

void UW_ItemImg::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (backgroundBorder)
	{
		// 마우스가 나갔을 때 보더 색상을 변경
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
		dragDropOperation->DefaultDragVisual = this;  // 드래그 시 보일 위젯 설정
		dragDropOperation->Pivot = EDragPivot::CenterCenter;

		// 드래그를 시작하면 인벤토리에서 제거된 것으로 간주하고 remove

		/*dragDropOperation->OnDragCancelled.AddDynamic(this, &UW_ItemImg::OnDragCancelled);
		dragDropOperation->OnDrop.AddDynamic(this, &UW_ItemImg::OnDragDrop);*/

		OutOperation = dragDropOperation;

		// 현재 위젯을 부모에서 제거
		// RemoveFromParent();
	}

}

// 마우스 누르면 드래그 감지
FReply UW_ItemImg::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 왼쪽 마우스 눌렸을 때 드래그 이벤트 감지
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

// 인벤토리를 다시 그릴 때마다 호출
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
	// 델리게이트 호출 - 파라미터 전달
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

