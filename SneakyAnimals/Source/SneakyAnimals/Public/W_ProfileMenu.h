// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataStructure.h"
#include "W_ProfileMenu.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_ProfileMenu : public UUserWidget
{
	GENERATED_BODY()
public:

    UW_ProfileMenu(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* CurrentAvatar_Image;
    
    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
    class UTextBlock* Username_Text;

    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
    class UW_Base_Textbox* Username_Textbox;

    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
    class UWrapBox* AvailableAvatars_WrapBox;

    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
    class UW_Base_Button* Save_Btn;

    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
    class UW_Base_Button* Cancel_Btn;

    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
    class UW_Base_Button* Back_Btn;

    UPROPERTY(VisibleAnywhere)
    UUserWidget* ParentWidget;

    UPROPERTY(VisibleAnywhere)
    FName Selected_Avatar_Name;

	UPROPERTY(VisibleAnywhere)
	UTexture2D* Selected_Avatar;

	UPROPERTY(VisibleAnywhere)
	FText New_Username;

    UFUNCTION()
    void Set_SelectedAvatar(FName Avatar_Name);

    void SetParentWidget(UUserWidget* InParentWidget);

    UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class UW_AvailableAvatar> AvailableAvatar_bp;

    UPROPERTY(EditAnywhere, Category = "MySettings", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UW_Tooltip> Tooltip_bp;

	UPROPERTY()
	class UW_AvailableAvatar* AvailableAvatar_inst;

	UPROPERTY(EditAnywhere, Category = "MySettings", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UW_Tooltip* Tooltip_inst;

    UFUNCTION()
    void OnUsername_Textbox_Changed(const FText& Text);
    
    UFUNCTION()
    void OnSave_BtnClicked();
    
    UFUNCTION()
    void OnBack_BtnClicked();
    
    UFUNCTION()
    void OnCancel_BtnClicked();

protected:
    virtual void NativeConstruct() override;
    void Populate_AvailableAvatars();


private:
    TObjectPtr<UDataTable> DT_Available_Avatars;
    FStructure_Available_Avatars* S_Available_Avatars;
    FStructure_UserProfile S_UserProfile;
    
    void Get_UserProfile();
};
