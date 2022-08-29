// Fill out your copyright notice in the Description page of Project Settings.


#include "Transformer.h"
#include "../BallPawn/BallPawn.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
// Sets default values
ATransformer::ATransformer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = Cube;

	MaterialTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("MaterialTimeline"));
	CorrespondingCollection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), nullptr, TEXT("/Game/BallPawn/MPC_Morph.MPC_Morph")));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/BallPawn/FreeToLockCurve"));
	check(Curve.Succeeded());
	FloatCurve = Curve.Object;
}

// Called when the game starts or when spawned
void ATransformer::BeginPlay()
{
	Super::BeginPlay();
//	Cube->SetMaterial(0, PropertyMap[TransProperty]);
	FOnTimelineFloat MaterialTimelineTickDelegate;
	MaterialTimelineTickDelegate.BindUFunction(this, "OnMaterialTimelineTick");
	FOnTimelineEventStatic MaterialTimelineFinished;
	MaterialTimelineFinished.BindUFunction(this, "OnMaterialTimelineFinished");
	MaterialTimeLine->SetTimelineFinishedFunc(MaterialTimelineFinished);
	MaterialTimeLine->AddInterpFloat(FloatCurve, MaterialTimelineTickDelegate);
	MaterialTimeLine->SetTimelineLengthMode(TL_LastKeyFrame);
}

void ATransformer::OnMaterialTimelineFinished()
{
	Destroy();
}

void ATransformer::OnMaterialTimelineTick(const float Output)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(),
		CorrespondingCollection, CorrespondingName, 1 - Output);
}

// Called every frame
void ATransformer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ATransformer::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	ABallPawn* PlayerBall = Cast<ABallPawn>(Other);
	if (PlayerBall && !IsInChange)
	{
		//ProperyStatus cur;
		if (TransProperty == TransformerProperty::Fire)
		{
		//	cur = ProperyStatus::Fire;
			PlayerBall->UnLockMap[ProperyStatus::Fire] = true;
			PlayerBall->SetFire();
		}
		else if (TransProperty == TransformerProperty::Ice)
		{
			PlayerBall->UnLockMap[ProperyStatus::Ice] = true;
			PlayerBall->SetIce();
		}
		else if (TransProperty == TransformerProperty::Iron)
		{
			PlayerBall->UnLockMap[ProperyStatus::Iron] = true;
			PlayerBall->SetIron();
		}
		IsInChange = true;
		MaterialTimeLine->SetPlaybackPosition(0.0f, false);
		MaterialTimeLine->Play();

	}
}

UStaticMeshComponent* ATransformer::GetStaticMesh()
{
	return Cube;
}

