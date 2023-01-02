// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "Components/BoxComponent.h"
#include "ShootingFlight.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;
	// SetRootComponent(BoxComponent);

	BoxComponent->SetBoxExtent(FVector(50.0f, 2000.0f, 50.0f));
	
	BoxComponent->SetCollisionProfileName(TEXT("WallPreset"));
	// ������ �̺�Ʈ Ȱ��ȭ
	BoxComponent->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();

	// ������ �̺�Ʈ ��������Ʈ�� ������ �Լ� ����
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWall::OnOverlap);
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWall::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	// �÷��̾ ������ �浹 ����� ����
	AShootingFlight* player = Cast<AShootingFlight>(OtherActor);
	if (player != nullptr)
	{

	}
	else
	{
		OtherActor->Destroy();
	}
}

