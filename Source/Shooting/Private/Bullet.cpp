// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGameModeBase.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 충돌 영역 생성
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// BoxComponent를 RootComponent 로 설정
	RootComponent = BoxComponent;
	BoxComponent->SetBoxExtent(FVector(50.0f));
	BoxComponent->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));
	BoxComponent->SetCollisionProfileName(TEXT("BulletPreset"));

	// 스태틱 메시 생성
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// 스태틱 메시를 루트 컴포넌트에 할당
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0, 0, -50.0f));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// 충돌(overlap) 발생 시 실행할 함수를 연결
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);

	// 오버랩 이벤트가 발생하도록 설정
	BoxComponent->SetGenerateOverlapEvents(true);
	
	// 생성 후 3초 뒤에 제거
	// SetLifeSpan(3.0f);

	// 생성 후 3초 뒤에 제거, World의 TimerManager 이용
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ABullet::DestroyBulletSelf, 3.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 액터의 정면 방향으로 계속 이동
	// direction = FVector(1, 0, 0);
	direction = GetActorForwardVector();
	FVector dir = GetActorLocation() + direction * MoveSpeed * DeltaTime;
	SetActorLocation(dir);
}

// OtherActor 로 대상 액터 정보, HitComponent 로 충돌 대상 컴포넌트 정보?
void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	// 만일 충돌 대상이 Enemy 라면
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy != nullptr)
	{
		// 적이 있던 위치에 폭발 이펙트 생성
		FVector EnemyLocation = Enemy->GetActorLocation();
		FRotator EnemyRotation = Enemy->GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, EnemyLocation, EnemyRotation, true);

		// Enemy 제거
		Enemy->Destroy();

		// 게임 모드 안에 점수 1점 추가
		AGameModeBase* GameMode =  UGameplayStatics::GetGameMode(this);
		// AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		AShootingGameModeBase* ShootingGameMode = Cast<AShootingGameModeBase>(GameMode);
		ShootingGameMode->AddScore(10);
		// UE_LOG(LogTemp, Warning, TEXT("Score: %d"), ShootingGameMode->GetCurrentScore());

		// Bullet 스스로도 제거
		Destroy();
	}
}

void ABullet::DestroyBulletSelf() {
	Destroy();
}

