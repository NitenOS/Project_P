// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "childBase.h"
#include "adoChild.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API AadoChild : public AchildBase
{
	GENERATED_BODY()
	
public:
	AadoChild();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int navPointCount = 1;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
