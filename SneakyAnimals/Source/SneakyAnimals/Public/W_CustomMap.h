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
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* gridBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* gridCanvasPanel;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitializeWidget(UItemComponent* ItemComponent, float Tilesize);

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
    class UItemComponent* itemComponent;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
    float tileSize = 160.f;


private:
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CreateLineSegments();
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CreateVerticalLine();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CreateHorizantalLine();
	
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TArray<class ULineStructure*> lines;

	/*UFUNCTION(BlueprintCallable, Category = "Custom")
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;*/

};
