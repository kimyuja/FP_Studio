// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	TArray<class ULineStructure*> lines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float tileSize = 160.f;

	FTimerHandle SetGridSizeTimerHandle;
	FTimerHandle DrawGridLineTimerHandle;

public:
	/*UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	float tileSize = 160.f;*/
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


};
