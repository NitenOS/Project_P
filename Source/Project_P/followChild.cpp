// Fill out your copyright notice in the Description page of Project Settings.


#include "followChild.h"

AfollowChild::AfollowChild() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//walkSFX setup
	WalkSFX = CreateDefaultSubobject<UAudioComponent>(TEXT("walkSFX"));
	WalkSFX->SetRelativeLocation(FVector(0.0f, 0.0f, -70.0f));
	WalkSFX->SetupAttachment(RootComponent);

	MusicChase = CreateDefaultSubobject<UAudioComponent>(TEXT("musicChase"));
	MusicChase->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
	MusicChase->SetupAttachment(RootComponent);
}

void AfollowChild::BeginPlay() {
	Super::BeginPlay();

	AActor* mainCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), AtestCharacter::StaticClass());
	mc = Cast<AtestCharacter>(mainCharacter);
	FollowPlayer();
	moveChild();
	MusicChase->Play();
	WalkSFX->Play();
}

void AfollowChild::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (mc->humainForm) {
		//Search player
		if (searchPlayer == true) {
			if (moveResult == EPathFollowingRequestResult::AlreadyAtGoal) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString(TEXT("On a trouvé le MC")));
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
		if (goChild == true) {
			countGo += DeltaTime;
			if (countGo >= maxCountGo) {
				goChild = false;
			}
		}

		// Timer end back
		if (countGo >= maxCountGo && goChild == false) {
			isChasing = false;
			setGoalLocation(FNavLocation(goBack));
			moveResult = MoveIA(goalLocation);
			if (moveResult == EPathFollowingRequestResult::AlreadyAtGoal) {
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString(TEXT("test")));

				//this->Destroy();
				this->SetActorLocation(goHidle);
				countGo = 0.0f;
				searchPlayer = true;
				FollowPlayer();
				moveResult = MoveIA(goalLocation);
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(isChasing));
	} else {

		goalLocation = FNavLocation(navPoints[navPointCount]);
		moveResult = MoveIA(goalLocation);
		if (moveResult == EPathFollowingRequestResult::AlreadyAtGoal) {
			moveResult = MoveIA(goalLocation);
			OnMoveCompleted(moveResult);
		}
	}


	// Footstep 
	{
		WalkSFX->Sound = footsteps[numFootstep];

		footstepcount += DeltaTime;
		if (footstepcount >= maxFootstepCount) {

			//numFootstep = FMath::RandRange(0, footsteps.Num() - 1);
			numFootstep += 1;
			if (numFootstep >= footsteps.Num()) { numFootstep = 0; }
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(numFootstep));

			WalkSFX->Play();
			footstepcount = 0.0f;
		}

	}

}

bool AfollowChild::FollowPlayer() {
	bool isOnPlayer = false;

	setGoalLocation(FNavLocation(mc->GetActorLocation()));

	return isOnPlayer;
}