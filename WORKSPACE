new_http_archive(
	name = "gmock_archive",
	url = "https://googlemock.googlecode.com/files/gmock-1.7.0.zip",
	sha256 = "26fcbb5925b74ad5fc8c26b0495dfc96353f4d553492eb97e85a8a6d2f43095b",
	build_file = "bazel/gmock.BUILD",
)

bind(
	name = "gmock",
	actual = "@gmock_archive//:gmock",
)

git_repository(
	name = "protobuf_repository",
	remote = "https://github.com/google/protobuf.git",
	commit = "48ebb29a8ec118bf6b9ee39f6be42b57321c099a",
	init_submodules = 1,
)

bind(
	name = "protobuf",
	actual = "@protobuf_repository//:protobuf",
)

#bind(
#	name = "protoc",
#	actual = "@protobuf_repository//:protoc",
#)
