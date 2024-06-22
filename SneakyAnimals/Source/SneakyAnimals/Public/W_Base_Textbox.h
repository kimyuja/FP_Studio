#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Base_Textbox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyTextChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyTextCommitted);

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Base_Textbox : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UEditableTextBox* Textbox;
	
	UPROPERTY(EditAnywhere, Category = "MyTextbox Configuration", BlueprintReadWrite)
	float Widget_Width;
	
	UPROPERTY(EditAnywhere, Category = "MyTextbox Configuration", BlueprintReadWrite)
	float Widget_Height;

	UPROPERTY(EditAnywhere, Category = "MyTextbox Configuration", BlueprintReadWrite)
	FText HintText;
	
	UPROPERTY(EditAnywhere, Category = "MyTextbox Configuration", BlueprintReadWrite)
	FText Text;
	
	UPROPERTY(EditAnywhere, Category = "MyTextbox Configuration", BlueprintReadWrite)
	FText ToolTip_Text;

	UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMyTextChanged OnMyTextChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMyTextCommitted OnMyTextCommitted;

	UFUNCTION()
    void OnTextChanged(const FText& InText);
	
	UFUNCTION()
    void OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

protected:
	//virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
};
