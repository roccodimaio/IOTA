// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUserName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConnectionFraction;

	void Setup(class UMainMenu* InParent, uint32 InIndex); 

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false; 
	
private:

	UPROPERTY()
	class UMainMenu* Parent;

	uint32 Index; 
	
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UFUNCTION()
	void OnClicked();
};
