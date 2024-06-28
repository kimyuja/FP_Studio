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

UW_CustomMap::UW_CustomMap(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	/*AActor* TempActor = GetWorld()->SpawnActor<AActor>();
	if (TempActor)
	{
		itemComponent = NewObject<UItemComponent>(TempActor, UItemComponent::StaticClass());
		if (itemComponent)
		{
			TempActor->AddOwnedComponent(itemComponent);
		}
		TempActor->Destroy();
	}

	if (!itemComponent)
	{
		itemComponent = NewObject<UItemComponent>(this, TEXT("ItemComponent"));

	}*/
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
	// UItemComponent* itemComponent = Cast<UItemComponent>()

	//if (itemComponent)
	//{
	//	// itemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("itemComponent"));
	//}

	if (gridBorder && gridCanvasPanel)
	{
		gridBorder->SetContent(gridCanvasPanel);
	}

	/*for (int i = 0; i < 10; ++i) {
		UE_LOG(LogTemp,  Warning, TEXT("%d "), i);
	}*/

}

void UW_CustomMap::InitializeWidget(float Tilesize)
{
	// UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(gridBorder->Slot);
	canvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(gridBorder);

	if (canvasSlot)
	{

		float sizeX = columns * tileSize;
		float sizeY = rows * tileSize;

		// float sizeX = itemComponent->columns * tileSize;
		// float sizeY = itemComponent->rows * tileSize;

		canvasSlot->SetSize(FVector2D(sizeX, sizeY));

	}

	// CreateLineSegments();
}

void UW_CustomMap::CreateLineSegments()
{
	CreateVerticalLine();
	CreateHorizantalLine();

}

void UW_CustomMap::CreateVerticalLine()
{
	float X;
	
	ULineStructure newLine;

	for (int32 i = 0; i < columns; ++i) {
		X = tileSize * i;

		newLine.start = FVector2D(X, 0.0f);
		newLine.end = FVector2D(X, tileSize * rows);

		lines.Add(&newLine);
	}
}

void UW_CustomMap::CreateHorizantalLine()
{
	float Y;
	
	ULineStructure newline;

	for (int32 i = 0; i < rows; ++i) {
		Y = tileSize * i;

		newline.start = FVector2D(0.0f, Y);
		newline.end = FVector2D(tileSize * columns, Y);

		lines.Add(&newline);
	}
}

// int32 UW_CustomMap::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled)
// {


	// return RetLayerId;
// }

void UW_CustomMap::DrawGridLine()
{
	FVector2D TopLeft = canvasSlot->GetPosition();
	UE_LOG(LogTemp, Warning, TEXT("TopLeft position : (%f, %f)"), TopLeft.X, TopLeft.Y);

	// FVector2D TopLeft = gridBorder->GetCachedGeometry().GetLocalTopLeft();


}
