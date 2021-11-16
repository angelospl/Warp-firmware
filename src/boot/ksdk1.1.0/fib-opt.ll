; ModuleID = '<stdin>'
source_filename = "fib"

%fibonacci_0.Frame = type { void (%fibonacci_0.Frame*)*, void (%fibonacci_0.Frame*)*, i32, i32, i32, i1 }

; Function Attrs: nounwind
define i32 @fib_gen(i32 %0) local_unnamed_addr #0 {
entry:
  %k_ltRes2 = icmp sgt i32 %0, 0
  br i1 %k_ltRes2, label %then.preheader, label %after

then.preheader:                                   ; preds = %entry
  %k_ltRes.peel.not = icmp eq i32 %0, 1
  br i1 %k_ltRes.peel.not, label %after, label %then

then:                                             ; preds = %then.preheader, %then
  %val_a_0.reload.i31 = phi i32 [ %val_b_02.reload8.i30, %then ], [ 0, %then.preheader ]
  %val_b_02.reload8.i30 = phi i32 [ %k_sumRes.i, %then ], [ 1, %then.preheader ]
  %var_i.03 = phi i32 [ %k_sumRes, %then ], [ 1, %then.preheader ]
  %k_sumRes.i = add i32 %val_a_0.reload.i31, %val_b_02.reload8.i30
  %k_sumRes = add nuw nsw i32 %var_i.03, 1
  %k_ltRes = icmp slt i32 %k_sumRes, %0
  br i1 %k_ltRes, label %then, label %after, !llvm.loop !0

after:                                            ; preds = %then, %then.preheader, %entry
  %var_fibRet.0.lcssa = phi i32 [ undef, %entry ], [ 1, %then.preheader ], [ %k_sumRes.i, %then ]
  ret i32 %var_fibRet.0.lcssa
}

; Function Attrs: nounwind
define noalias nonnull i8* @fibonacci_0() #0 {
entry:
  %malloccall = tail call dereferenceable_or_null(32) i8* @malloc(i32 32)
  %resume.addr = bitcast i8* %malloccall to void (%fibonacci_0.Frame*)**
  store void (%fibonacci_0.Frame*)* @fibonacci_0.resume, void (%fibonacci_0.Frame*)** %resume.addr, align 8
  %destroy.addr = getelementptr inbounds i8, i8* %malloccall, i64 8
  %0 = bitcast i8* %destroy.addr to void (%fibonacci_0.Frame*)**
  store void (%fibonacci_0.Frame*)* @fibonacci_0.destroy, void (%fibonacci_0.Frame*)** %0, align 8
  %index.addr9 = getelementptr inbounds i8, i8* %malloccall, i64 28
  %1 = bitcast i8* %index.addr9 to i1*
  store i1 false, i1* %1, align 1
  ret i8* %malloccall
}

; Function Attrs: inaccessiblememonly mustprogress nofree nounwind willreturn
declare noalias noundef i8* @malloc(i32 noundef) local_unnamed_addr #1

; Function Attrs: inaccessiblemem_or_argmemonly mustprogress nounwind willreturn
declare void @free(i8* nocapture noundef) local_unnamed_addr #2

; Function Attrs: nounwind
define internal fastcc void @fibonacci_0.resume(%fibonacci_0.Frame* noalias nonnull align 8 dereferenceable(32) %FramePtr) #0 {
entry.resume:
  %k_retPromise.reload.addr = getelementptr inbounds %fibonacci_0.Frame, %fibonacci_0.Frame* %FramePtr, i64 0, i32 2
  %index.addr = getelementptr inbounds %fibonacci_0.Frame, %fibonacci_0.Frame* %FramePtr, i64 0, i32 5
  %index = load i1, i1* %index.addr, align 4
  br i1 %index, label %loop.from.coroResume.critedge, label %loop

loop:                                             ; preds = %entry.resume, %loop.from.coroResume.critedge
  %val_a_0 = phi i32 [ %val_b_02.reload8, %loop.from.coroResume.critedge ], [ 0, %entry.resume ]
  %val_b_02 = phi i32 [ %k_sumRes, %loop.from.coroResume.critedge ], [ 1, %entry.resume ]
  %val_b_02.spill.addr = getelementptr inbounds %fibonacci_0.Frame, %fibonacci_0.Frame* %FramePtr, i64 0, i32 4
  store i32 %val_b_02, i32* %val_b_02.spill.addr, align 8
  %val_a_0.spill.addr = getelementptr inbounds %fibonacci_0.Frame, %fibonacci_0.Frame* %FramePtr, i64 0, i32 3
  store i32 %val_a_0, i32* %val_a_0.spill.addr, align 4
  store i32 %val_b_02, i32* %k_retPromise.reload.addr, align 8
  %index.addr10 = getelementptr inbounds %fibonacci_0.Frame, %fibonacci_0.Frame* %FramePtr, i64 0, i32 5
  store i1 true, i1* %index.addr10, align 4
  ret void

loop.from.coroResume.critedge:                    ; preds = %entry.resume
  %val_b_02.reload.addr7 = getelementptr inbounds %fibonacci_0.Frame, %fibonacci_0.Frame* %FramePtr, i64 0, i32 4
  %val_b_02.reload8 = load i32, i32* %val_b_02.reload.addr7, align 8
  %val_a_0.reload.addr = getelementptr inbounds %fibonacci_0.Frame, %fibonacci_0.Frame* %FramePtr, i64 0, i32 3
  %val_a_0.reload = load i32, i32* %val_a_0.reload.addr, align 4
  %k_sumRes = add i32 %val_b_02.reload8, %val_a_0.reload
  br label %loop
}

; Function Attrs: nounwind
define internal fastcc void @fibonacci_0.destroy(%fibonacci_0.Frame* noalias nonnull align 8 dereferenceable(32) %FramePtr) #0 {
entry.destroy:
  %vFrame = bitcast %fibonacci_0.Frame* %FramePtr to i8*
  tail call void @free(i8* nonnull %vFrame)
  ret void
}

attributes #0 = { nounwind }
attributes #1 = { inaccessiblememonly mustprogress nofree nounwind willreturn }
attributes #2 = { inaccessiblemem_or_argmemonly mustprogress nounwind willreturn }

!0 = distinct !{!0, !1}
!1 = !{!"llvm.loop.peeled.count", i32 1}
