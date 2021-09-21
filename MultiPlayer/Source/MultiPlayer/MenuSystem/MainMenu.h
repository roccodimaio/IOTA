// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"


USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString ServerName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName; 
};

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerData);

	void SelectIndex(uint32 Index);

private: 

	// Variables and components
	UPROPERTY(meta = (BindWidget))
	class UButton* Host_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostByServer_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinMenu_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinByIP_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitMainMenu_Button;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	TSubclassOf<class UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedIndex;

	//UPROPERTY(meta = (BindWidget))
	//class UEditableTextBox* IPAddressField;

/**
public:
	void SetMenuInterface(IMenuInterface* SetMenuInterface);

	void Setup();
	void Teardown();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

*/

protected:

	// Functions
	virtual bool Initialize();

private:
	
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenHostMenu();
	
	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ExitMainMenu();

	void UpdateChildren(); 

	//IMenuInterface* MenuInterface;
	
};
