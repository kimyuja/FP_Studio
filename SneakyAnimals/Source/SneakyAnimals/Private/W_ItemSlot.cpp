// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ItemSlot.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "Slate/SlateBrushAsset.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Kismet/KismetStringLibrary.h"
#include "UITestCharacter.h"
#include "MapCustomWidget.h"
#include "ItemComponent.h"
#include "ItemObject.h"
#include "Components/TextBlock.h"
#include "WH_BookshelfGimmick.h"
#include "WH_BroomstickGimmick.h"
#include "WH_PotionGimmick.h"
#include "WH_WitchCauldronGimmick.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/AssetManager.h>
#include "W_CustomMap.h"
#include <ItemManager.h>
#include "NewGridWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <TestPlayer.h>
#include "GimmickSelectionWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanelSlot.h>

bool UW_ItemSlot::Initialize()
{

	bool Success = Super::Initialize();
	if (!Success) return false;


	return true;
}

void UW_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!playerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("playerCharacter is nullptr"));
	}

	ATestPlayer* TestPlayer = Cast<ATestPlayer>(playerCharacter);


	if (!TestPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestPlayer is nullptr"));
	}

	if (TestPlayer)
	{
		itemComponent = TestPlayer->FindComponentByClass<UItemComponent>();

		UE_LOG(LogTemp, Warning, TEXT("TestPlayer connect itemComponent"));
	}

	if (itemBtn)
	{
		itemBtn->OnClicked.AddDynamic(this, &UW_ItemSlot::OnItemBtnClicked);
	}

	if (!mapCustomWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("mapCustomWidget is not set!"));
	}

	itemObject = NewObject<UItemObject>(this);

	if (!itemObject)
	{
		UE_LOG(LogTemp, Error, TEXT("itemObject is not set!"));
	}

	gridWidget = CreateWidget<UNewGridWidget>(GetWorld(), newGridWidget);
	
}


void UW_ItemSlot::OnItemBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Item button clicked!"));
	
	UE_LOG(LogTemp, Warning, TEXT("this itemtype is %d"), this->itemType);
}


void UW_ItemSlot::SetMapCustomWidget(UMapCustomWidget* Widget)
{
	mapCustomWidget = Widget;

	if (!mapCustomWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("mapCustomWidget is null!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("mapCustomWidget is set!"));
	}
}

void UW_ItemSlot::SetItemIcon(const FString& TexturePath)
{
	// 텍스처 로드
	UTexture2D* ItemTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
	if (ItemTexture && itemIcon)
	{
		// 이미지 위젯의 브러시 설정
		itemIcon->SetBrushFromTexture(ItemTexture);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load texture or itemIcon is null"));
	}

}

void UW_ItemSlot::InitializeItemSlot(int32 _ItemType)
{
	switch (_ItemType)
	{
	case 0:
	{
		FString fromIcon1 = TEXT("/Game/RTY/Texture/Icon/bookShelf");
		SetItemIcon(fromIcon1);
		itemIcon->SetRenderScale(FVector2D(1.4f, 1.4f));
		break;
	}
	case 1:
	{
		FString fromIcon2 = TEXT("/Game/RTY/Texture/Icon/broomStick.broomStick");
		SetItemIcon(fromIcon2);
		break;
	}
	case 2:
	{
		FString fromIcon3 = TEXT("/Game/RTY/Texture/Icon/Table.Table");
		SetItemIcon(fromIcon3);
		break;
	}
	case 3:
	{
		FString fromIcon4 = TEXT("/Game/RTY/Texture/Icon/pot.pot");
		SetItemIcon(fromIcon4);
		break;
	}

	}
}

void UW_ItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Enter"));
	OnMouseEnterWidget();
}

void UW_ItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Leave"));
	OnMouseLeaveWidget();
}

void UW_ItemSlot::OnMouseEnterWidget()
{
	UCanvasPanel* rootCanvas = Cast<UCanvasPanel>(GetRootWidget());

	gimmickSelectionWidget = CreateWidget<UGimmickSelectionWidget>(GetWorld(), GimmickSelectionWidgetClass);

	if (gimmickSelectionWidget)
	{

		UCanvasPanelSlot* canvasSlot = rootCanvas->AddChildToCanvas(gimmickSelectionWidget);

		if (canvasSlot)
		{
			canvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));

			FVector2D SelectWPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - FVector2D(100, 0);

			canvasSlot->SetPosition(FVector2D(-450.f, -100.f));
			canvasSlot->SetSize(FVector2D(450.f, 350.f));


			UE_LOG(LogTemp, Warning, TEXT("gimmickSelectionWidget created and positioned at (%f, %f)"), SelectWPosition.X, SelectWPosition.Y);

			UE_LOG(LogTemp, Warning, TEXT("gimmickSelectionWidget size: (%f, %f)"), canvasSlot->GetSize().X, canvasSlot->GetSize().Y);
			
			gimmickSelectionWidget->BindItemType(itemType);

			// 위젯 내에서 마우스가 떠나는 이벤트 처리
			gimmickSelectionWidget->OnCustomMouseLeave.AddDynamic(this, &UW_ItemSlot::OnSelectionWidgetMouseLeave);

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GimmickSelectionWidget"));
	}

}

void UW_ItemSlot::OnMouseLeaveWidget()
{
	if (gimmickSelectionWidget)
	{
		gimmickSelectionWidget->RemoveFromParent();
		gimmickSelectionWidget = nullptr;

	}
}

void UW_ItemSlot::OnSelectionWidgetMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (gimmickSelectionWidget)
	{
		gimmickSelectionWidget->RemoveFromParent();
		gimmickSelectionWidget = nullptr;

	}
}



