// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "testCharacter.h"
#include "vaiselleTask.generated.h"

UCLASS()
class PROJECT_P_API AvaiselleTask : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AvaiselleTask();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float taskProgession = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
