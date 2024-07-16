// Fill out your copyright notice in the Description page of Project Settings.


#include "W_CustomMap.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include <ItemComponent.h>
#include "LineStructure.h"
#include <../../../../../../../Source/Runtime/Experimental/ChaosCore/Public/Chaos/Array.h>
#include "Slate/WidgetTransform.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include "Rendering/DrawElements.h"
#include "Layout/Geometry.h"
#include "TileStructure.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <UITestCharacter.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>
#include "W_ItemImg.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
#include <ItemManager.h>
#include "TestPlayer.h"

UW_CustomMap::UW_CustomMap(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UW_CustomMap::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!gridBorder) return false;

	gridBorder->SetPadding(0.0f);


	return true;
}

void UW_CustomMap::NativeConstruct()
{
	Super::NativeConstruct();


	/*
	if (itemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("itemComponent is not nullptr"));
	}*/

	if (gridBorder && gridCanvasPanel)
	{
		gridBorder->SetContent(gridCanvasPanel);
	}

	Invalidate(EInvalidateWidget::LayoutAndVolatility);

	GetWorld()->GetTimerManager().SetTimer(DrawGridLineTimerHandle, this, &UW_CustomMap::DrawGridLine, 0.1f, false);

	/*UE_LOG(LogTemp, Warning, TEXT("itemComponent->columns : %d, itemComponent->rows : %d"), itemComponent->columns, itemComponent->rows);*/

	// DrawGridLine();

	//Refresh();

	//itemComponent->OnInventoryChanged.AddDynamic(this, &UW_CustomMap::Refresh);
}

int32 UW_CustomMap::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 RetLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FVector2D topLeft = GetGridBorderTopLeft();

	// Draw each line
	for (const ULineStructure* Line : lines)
	{
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			RetLayerId,
			AllottedGeometry.ToPaintGeometry(),
			TArray<FVector2D>({ topLeft + Line->start, topLeft + Line->end }),
			ESlateDrawEffect::None,
			FLinearColor::Gray,
			true,
			1.0f
		);
	}

	return RetLayerId;

}

void UW_CustomMap::InitializeWidget(float _Tilesize)
{
	// UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(gridBorder->Slot);
	
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	/*if (!playerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("playerCharacter is nullptr"));
	}*/

	ATestPlayer* TestPlayer = Cast<ATestPlayer>(playerCharacter);
	if (!TestPlayer)
	{
		// UE_LOG(LogTemp, Warning, TEXT("uiTestPlayer is nullptr"));
	}
	if (TestPlayer)
	{
		itemComponent = TestPlayer->FindComponentByClass<UItemComponent>();
		// UE_LOG(LogTemp, Warning, TEXT("uiTestPlayer is not nullptr"));
	}
	canvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(gridBorder);
	
	if (canvasSlot)
	{

		/*float sizeX = itemComponent->columns * tileSize;
		float sizeY = itemComponent->rows * tileSize;*/
		float sizeX = itemComponent->columns * _Tilesize;
		float sizeY = itemComponent->rows * _Tilesize;
		UE_LOG(LogTemp, Warning, TEXT("%f, %f"), sizeX, sizeY);
		canvasSlot->SetSize(FVector2D(sizeX, sizeY));
	}
	UE_LOG(LogTemp, Warning, TEXT("Draw"));

	Refresh(); // 맨 처음 초기화
	
	itemComponent->OnInventoryChanged.AddDynamic(this, &UW_CustomMap::Refresh);
}

// 그리드가 변경 될 때마다 이 함수가 호출되어야 함
void UW_CustomMap::Refresh()
{
	UE_LOG(LogTemp, Warning, TEXT("Refresh Func Start"));

	if (gridCanvasPanel)
	{
		gridCanvasPanel->ClearChildren();

	}
	else if (!gridCanvasPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("gridcanvaspanel val is nullptr"));
	}

	// UItemManager* ItemManager = UItemManager::Get();
	
	TMap<UItemObject*, FTileStructureTemp> allItems = itemComponent->GetAllItems();

	// 여기서부터
	// ForEachItme 이라는 매크로로 사용
	TArray<UItemObject*> Keys;
	allItems.GenerateKeyArray(Keys);

	for (UItemObject* key : Keys)
	{
		FTileStructureTemp* topLeftTile = allItems.Find(key);
		UItemObject* itemObejct = key;
		// 여기까지
		
		// item을 나타내는 위젯 생성
		UW_ItemImg* newItemImg = CreateWidget<UW_ItemImg>(GetWorld(), ItemImgWidgetClass);

		if (newItemImg)
		{
			newItemImg->tileSize = tileSize;
			newItemImg->thisItemObject = itemObejct;


			if (newItemImg)
			{
				// 델리게이트 바인딩
				newItemImg->OnRemoved.AddDynamic(this, &UW_CustomMap::OnItemRemoved);
 
				// UCanvasPanelSlot* tempCanvasSlot = Cast<UCanvasPanelSlot>(gridCanvasPanel->AddChild(newItemImg));

				UCanvasPanelSlot* tempCanvasSlot = Cast<UCanvasPanelSlot>(gridCanvasPanel->AddChildToCanvas(newItemImg));

				if (tempCanvasSlot)
				{
					// tempCanvasSlot->SetAutoSize(true);
					tempCanvasSlot->SetSize(FVector2D(itemObejct->dimensions.X * tileSize, itemObejct->dimensions.Y * tileSize));
					
					tempCanvasSlot->SetPosition(FVector2D(topLeftTile->X * tileSize, topLeftTile->Y*tileSize));

				}

				UE_LOG(LogTemp, Warning, TEXT("position: (%f, %f)"), tempCanvasSlot->GetPosition().X, tempCanvasSlot->GetPosition().Y);
		
				UE_LOG(LogTemp, Warning, TEXT("size : (%f, %f)"), tempCanvasSlot->GetSize().X, tempCanvasSlot->GetSize().Y);
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("fail to new item image widget create"));
		}
		
	}
}

void UW_CustomMap::OnItemRemoved(class UItemObject* _ItemObject)
{
	itemComponent->RemoveItem(_ItemObject);
}

void UW_CustomMap::CreateLineSegments()
{
	lines.Empty();
	CreateVerticalLine();
	CreateHorizantalLine();

}

void UW_CustomMap::CreateVerticalLine()
{
	float X;

	for (int32 i = 0; i <= columns; ++i) {
		X = tileSize * i;

		ULineStructure* newLine = NewObject<ULineStructure>();
		newLine->start = FVector2D(X, 0.0f);
		newLine->end = FVector2D(X, tileSize * rows);

		lines.Add(newLine);
	}
}

void UW_CustomMap::CreateHorizantalLine()
{
	float Y;

	for (int32 i = 0; i <= rows; ++i) {
		Y = tileSize * i;

		ULineStructure* newLine = NewObject<ULineStructure>();
		newLine->start = FVector2D(0.0f, Y);
		newLine->end = FVector2D(tileSize * columns, Y);

		lines.Add(newLine);
	}
}


void UW_CustomMap::DrawGridLine()
{

	CreateLineSegments();

	if (IsInViewport())
	{
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}

}

FVector2D UW_CustomMap::GetGridBorderTopLeft() const
{

	FGeometry geometry = gridBorder->GetCachedGeometry();
	FVector2D absolutePosition = geometry.GetAbsolutePosition();
	FVector2D localPosition = geometry.AbsoluteToLocal(absolutePosition);
	FVector2D borderSize = geometry.GetLocalSize();

	
	FVector2D topLeft = localPosition;

	return topLeft;
}
