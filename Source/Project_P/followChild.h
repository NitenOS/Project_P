// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "childBase.h"
#include "testCharacter.h"
#include "followChild.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API AfollowChild : public AchildBase
{
	GENERATED_BODY()
	
public:
	AfollowChild();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	FVector goBack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	FVector goHidle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AtestCharacter* mc;

	bool searchPlayer = true;



	bool goChild = false;
	float countGo = 0.0f;
	float maxCountGo = 2.0f;
 
	bool FollowPlayer();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
