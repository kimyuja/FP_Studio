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
}

