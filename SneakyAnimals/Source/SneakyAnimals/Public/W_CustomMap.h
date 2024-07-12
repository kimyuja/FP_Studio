// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LineStructure.h"
#include "W_CustomMap.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_CustomMap : public UUserWidget
{
	GENERATED_BODY()

public:
	UW_CustomMap(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* gridBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* gridCanvasPanel;
	
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UW_ItemImg* itemImageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUserWidget> ItemImgWidgetClass;


	UPROPERTY(EditAnywhere)
	class UCanvasPanelSlot* canvasSlot;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitializeWidget(float Tilesize);

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
    class UItemComponent* itemComponent;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
    float tileSize = 160.f;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	int32 columns = 5;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	int32 rows = 5;

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void Refresh();


private:
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CreateLineSegments();
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CreateVerticalLine();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CreateHorizantalLine();
	
	UPROPERTY(VisibleAnywhere, Category = "Custom")
	TArray<class ULineStructure*> lines;


	UFUNCTION(BlueprintCallable, Category = "Custom")
	void DrawGridLine();

	FVector2D GetGridBorderTopLeft() const;

	FTimerHandle DrawGridLineTimerHandle;

public:

	// 델리게이트 바인딩 할 함수
	UFUNCTION(BlueprintCallable)
	void OnItemRemoved(class UItemObject* _ItemObject);



};
