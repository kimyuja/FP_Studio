// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ItemImg.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SizeBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include "timerManager.h"
#include "ItemObject.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanelSlot.h>
#include "W_CustomMap.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include <W_ItemSlot.h>


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

// 인벤토리를 다시 그릴 때마다 호출
void UW_ItemImg::Refresh()
{
	
	if (itemObject != nullptr)
	{
		size.X = itemObject->GetDimensions().X * tileSize;
		size.Y = itemObject->GetDimensions().Y * tileSize;

		backgroundSizeBox->SetWidthOverride(size.X);
		backgroundSizeBox->SetHeightOverride(size.Y);
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
		}

		itemImage->SetBrush(GetIconImage());
	}


}

FSlateBrush UW_ItemImg::GetIconImage() const
{
	FSlateBrush brush;

	if (itemObject)
	{
		UMaterialInterface* iconMaterial = itemObject->GetIcon();

		if (iconMaterial)
		{
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
}

