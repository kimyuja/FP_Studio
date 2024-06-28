#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Base_OptionSwitcher.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyOptionSwitched);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyOptionSwitched, const FString&, SelectedOption);

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Base_OptionSwitcher : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UW_Base_OptionSwitcher(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UButton* Left_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UButton* Right_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class USizeBox* SizeBox;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class USizeBox* SizeBox_Left_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class USizeBox* SizeBox_Right_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* TextBlock;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TArray<FString> Options;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FString SelectedOption;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bAllow_OptionLooping;

	UPROPERTY(EditAnywhere, Category = "My Style Configuration")
	float Widget_Width;

	UPROPERTY(EditAnywhere, Category = "My Style Configuration")
	float Widget_Height;

	UPROPERTY(EditAnywhere, Category = "My Style Configuration")
	float Left_Btn_Width;

	UPROPERTY(EditAnywhere, Category = "My Style Configuration")
	float Right_Btn_Width;

	UPROPERTY(EditAnywhere, Category = "My Style Configuration")
	int32 Font_Size;

	UFUNCTION()
	void OnLeft_BtnClicked();
	
	UFUNCTION()
	void OnRight_BtnClicked();

	UFUNCTION()
	void Setup();

	UFUNCTION()
	void Switch_Option(bool bDecrease);

	UFUNCTION()
	void Set_SelectedOption(FString _SelectedOption);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMyOptionSwitched OnMyOptionSwitched;

    UFUNCTION()
    void OnOptionSwitched(const FString& _SelectedOption);

protected:
	virtual void NativeConstruct() override;


};
