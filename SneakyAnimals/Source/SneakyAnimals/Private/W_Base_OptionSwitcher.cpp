#include "W_Base_OptionSwitcher.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SizeBox.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/Font.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetArrayLibrary.h>

UW_Base_OptionSwitcher::UW_Base_OptionSwitcher(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Options = { "Low", "Medium", "High", "Epic", "Ultra" };
	SelectedOption = "High";
	Widget_Width = 304.0;
	Widget_Height = 36.0;
	Left_Btn_Width = 33.0;
	Right_Btn_Width = 33.0;
	Font_Size = 11;
	bAllow_OptionLooping = false;
}

void UW_Base_OptionSwitcher::OnLeft_BtnClicked()
{
	Switch_Option(true);
}

void UW_Base_OptionSwitcher::OnRight_BtnClicked()
{
	Switch_Option(false);
}

void UW_Base_OptionSwitcher::Setup()
{
	if (Options.Contains(SelectedOption))
	{
		Set_SelectedOption(Options[Options.Find(SelectedOption)]);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Makes sure the [SelectedOption] is actually present in the [Options] array."));
	}

	// Set SizeBox
	SizeBox->SetWidthOverride(Widget_Width);
	SizeBox->SetHeightOverride(Widget_Height);
	SizeBox_Left_Btn->SetWidthOverride(Left_Btn_Width);
	SizeBox_Right_Btn->SetWidthOverride(Right_Btn_Width);

	// Load the Roboto font and set the font info
	FSlateFontInfo FontInfo;
	UFont* FontObject = LoadObject<UFont>(nullptr, TEXT("/Engine/EngineFonts/Roboto.Roboto"));
	if (FontObject)
	{
		FontInfo.FontObject = FontObject;
		FontInfo.Size = Font_Size;
		TextBlock->SetFont(FontInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load Roboto font"));
	}
}

void UW_Base_OptionSwitcher::Switch_Option(bool bDecrease)
{
	int32 Current_Selection = 0;
	int32 Min_Options = 0;
	// Sets the [Max Options] to the user defined [Options] array length -1. (-1 because indexes have a 0 based count)
	int32 Max_Options = Options.Num()-1;

	if (Options.Contains(SelectedOption))
	{
		// Sets the [Current Selection] index to the user defined [Selected Option]
		Current_Selection = Options.Find(SelectedOption);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[SelectedOption] is not present in the [Options] array."));
	}

	// This event switches the [Selected Option] to the Option on the left or right next to it depending on what the user clicks (Left or Right button). When [Allow_OptionLooping?] is set to true then the OptionSwitcher can loop both left and right through the options.
	if (bDecrease)
	{
		if (Current_Selection <= Min_Options)
		{
			if (bAllow_OptionLooping)
			{
				Current_Selection = Max_Options;
			}
			else
			{
				Current_Selection = Min_Options;
			}
		}
		else
		{
			// case : Current_Selection > Min_Options
			Current_Selection -= 1;
		}
	}
	else
	// case : bDecrease == false
	{
		if (Current_Selection >= Max_Options)
		{
			if (bAllow_OptionLooping)
			{
				Current_Selection = Min_Options;
			}
			else
			{
				Current_Selection = Max_Options;
			}
		}
		else
		{
			// case : Current_Selection < Max_Options
			Current_Selection += 1;
		}
	}

	// Finds the corresponding option from the [Options] array to the [Current_Selection] index. Then sets the [SelectedOption].
	Set_SelectedOption(Options[Current_Selection]);

	// delegate
	OnOptionSwitched(SelectedOption);

	return;

}

void UW_Base_OptionSwitcher::Set_SelectedOption(FString _SelectedOption)
{
	SelectedOption = _SelectedOption;
	TextBlock->SetText(UKismetTextLibrary::Conv_StringToText(SelectedOption));
}

void UW_Base_OptionSwitcher::OnOptionSwitched(const FString& _SelectedOption)
{
	OnMyOptionSwitched.Broadcast(_SelectedOption);
}

void UW_Base_OptionSwitcher::NativeConstruct()
{
	Super::NativeConstruct();
	
	Setup();

	// Button Event Bind
	if (Left_Btn)
	{
		Left_Btn->OnClicked.AddDynamic(this, &UW_Base_OptionSwitcher::OnLeft_BtnClicked);
	}
	if (Right_Btn)
	{
		Right_Btn->OnClicked.AddDynamic(this, &UW_Base_OptionSwitcher::OnRight_BtnClicked);
	}
}


