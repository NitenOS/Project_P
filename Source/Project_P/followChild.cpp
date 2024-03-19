// Fill out your copyright notice in the Description page of Project Settings.


#include "followChild.h"

AfollowChild::AfollowChild() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AfollowChild::BeginPlay() {
	Super::BeginPlay();

	AActor* mainCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), AtestCharacter::StaticClass());
	mc = Cast<AtestCharacter>(mainCharacter);
	FollowPlayer();
	moveChild();
}

void AfollowChild::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//Search player
	if (searchPlayer == true){
		if (moveResult == EPathFollowingRequestResult::AlreadyAtGoal) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString(TEXT("On a trouvé le MC")));
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, GetTargetLocation().ToString());
			mc->Shoked(this->GetActorRotation());
			goChild = true;
			searchPlayer = false;
		}
		FollowPlayer();
		moveResult = MoveIA(goalLocation);
	}

	if (mc->getIsHide()) {
		countGo = maxCountGo;
	}

	// Go child
	if (goChild == true){
		countGo += DeltaTime;
		if (countGo >= maxCountGo) {
			goChild = false;
		}
	}

	// Timer end back
	if (countGo >= maxCountGo && goChild == false) {
		setGoalLocation(FNavLocation(goBack));
		moveResult = MoveIA(goalLocation);
		if (moveResult == EPathFollowingRequestResult::AlreadyAtGoal) { this->Destroy(); }
	}

	

}

bool AfollowChild::FollowPlayer() {
	bool isOnPlayer = false;

	setGoalLocation(FNavLocation(mc->GetActorLocation()));

	return isOnPlayer;
}