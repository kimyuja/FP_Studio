
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Base_Button.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyButtonClicked);

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Base_Button : public UUserWidget
{
	GENERATED_BODY()

public:
	UW_Base_Button(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* TextBlock;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	UTexture2D* NormalImage;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	UTexture2D* HoveredImage;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	UTexture2D* PressedImage;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	FLinearColor FontColor;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	FText Button_Text;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	int32 Font_Size;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	float Button_Width;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	float Button_Height;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	TEnumAsByte<EHorizontalAlignment> Btn_HorizontalAlignment;

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	FMargin Btn_Padding;

	UFUNCTION(BlueprintCallable)
	void Setup();

	UFUNCTION(BlueprintCallable)
	void Set_ButtonStyle(UTexture2D* NewNormalImage, UTexture2D* NewHoveredImage, UTexture2D* NewPressedImage, FLinearColor NewFontColor);

	UFUNCTION(BlueprintCallable)
	void Set_ButtonSize();

	UFUNCTION(BlueprintCallable)
	FSlateBrush Get_SlateBrush(UTexture2D* Img);

	UFUNCTION(BlueprintCallable)
	void Set_Font();

	//--------------------Setup ³¡

	UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMyButtonClicked OnMyButtonClicked;

    UFUNCTION()
    void OnButtonClicked();

	class UFont* FontObject;
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;


private:
	FSlateBrush CreateSlateBrushFromTexture(UTexture2D* Texture) const;
	FButtonStyle CreateButtonStyle(const FSlateBrush& NormalBrush, const FSlateBrush& HoveredBrush, const FSlateBrush& PressedBrush) const;
	void SetTextBlockColor(FLinearColor NewFontColor);
};
