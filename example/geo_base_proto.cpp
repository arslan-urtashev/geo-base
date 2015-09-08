#include "proto/geo_data.pb.h"
#include "util.hpp"

#include <google/protobuf/descriptor.h>

#include <iostream>

#include <unistd.h>
#include <stdio.h>

using namespace std;
using namespace geo_base;
using namespace google::protobuf;

static ostream& operator << (ostream &out, vector<string> const &v)
{
	for (size_t i = 0; i < v.size(); ++i)
		out << (i == 0 || (v[i].front() == '[' && v[i].back() == ']') ? "" : ".") << v[i];
	return out;
}

static void ForEach(const Message* message, vector<string> &path)
{
	const Descriptor* d = message->GetDescriptor();
	const Reflection* r = message->GetReflection();

	for (int i = 0; i < d->field_count(); ++i) {
		const FieldDescriptor* fd = d->field(i);
		path.push_back(fd->name());

		if (!fd->options().GetExtension(proto::ENUM).empty()) {
			const EnumDescriptor *ed = d->FindEnumTypeByName(fd->options().GetExtension(proto::ENUM));
			assert(fd);

			cout << path << "=";

			uint64_t set = (fd->type() == FieldDescriptor::TYPE_UINT64 ? r->GetUInt64(*message, fd) : r->GetUInt32(*message, fd));
			bool sep = false;
			for (int i = 0; i < ed->value_count(); ++i) {
				if (set & (1ULL << i)) {
					if (sep)
						cout << ",";
					const EnumValueDescriptor *vd = ed->value(i);
					cout << vd->options().GetExtension(proto::JSON);
					sep = true;
				}
			}
			cout << "\t";

		} else if (fd->label() == FieldDescriptor::LABEL_OPTIONAL || fd->label() == FieldDescriptor::LABEL_REQUIRED) {
			switch (fd->type()) {
			case FieldDescriptor::TYPE_DOUBLE:
				cout << path << "=" << r->GetDouble(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_FLOAT:
				cout << path << "=" << r->GetFloat(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_INT64:
				cout << path << "=" << r->GetInt64(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_UINT64:
				cout << path << "=" << r->GetUInt64(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_INT32:
				cout << path << "=" << r->GetInt32(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_FIXED64:
				cout << path << "=" << r->GetUInt64(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_FIXED32:
				cout << path << "=" << r->GetUInt32(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_BOOL:
				cout << path << "=" << r->GetBool(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_STRING:
				cout << path << "=" << "\"" << r->GetStringReference(*message, fd, NULL) << "\"" << '\t';
				break;
			case FieldDescriptor::TYPE_GROUP:
				assert(false);
				break;
			case FieldDescriptor::TYPE_MESSAGE:
				ForEach(&(r->GetMessage(*message, fd)), path);
				break;
			case FieldDescriptor::TYPE_BYTES:
				cout << path << "=" << "\"" << r->GetStringReference(*message, fd, NULL) << "\"" << '\t';
				break;
			case FieldDescriptor::TYPE_UINT32:
				cout << path << "=" << r->GetUInt32(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_ENUM:
			{
				const EnumValueDescriptor *ed = r->GetEnum(*message, fd);
				string sql = ed->options().GetExtension(proto::JSON);
				cout << path << "=" << sql << '\t';
			}
				break;
			case FieldDescriptor::TYPE_SFIXED32:
				cout << path << "=" << r->GetUInt32(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_SFIXED64:
				cout << path << "=" << r->GetUInt64(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_SINT32:
				cout << path << "=" << r->GetInt32(*message, fd) << '\t';
				break;
			case FieldDescriptor::TYPE_SINT64:
				cout << path << "=" << r->GetInt64(*message, fd) << '\t';
				break;
			default:
				break;
			}

		} else if (fd->label() == FieldDescriptor::LABEL_REPEATED) {
			int count = r->FieldSize(*message, fd);
			for (int i = 0; i < count; ++i) {
				path.push_back("[" + to_string(i) + "]");
				switch (fd->type()) {
				case FieldDescriptor::TYPE_DOUBLE:
					cout << " = " << r->GetRepeatedDouble(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_FLOAT:
					cout << " = " << r->GetRepeatedFloat(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_INT64:
					cout << " = " << r->GetRepeatedInt64(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_UINT64:
					cout << " = " << r->GetRepeatedUInt64(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_INT32:
					cout << " = " << r->GetRepeatedInt32(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_FIXED64:
					cout << " = " << r->GetRepeatedUInt64(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_FIXED32:
					cout << " = " << r->GetRepeatedUInt32(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_BOOL:
					cout << " = " << r->GetRepeatedBool(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_STRING:
					cout << " = " << "\"" << r->GetRepeatedStringReference(*message, fd, i, NULL) << "\"" << '\t';
					break;
				case FieldDescriptor::TYPE_GROUP:
					assert(false);
					break;
				case FieldDescriptor::TYPE_MESSAGE:
					ForEach(&(r->GetRepeatedMessage(*message, fd, i)), path);
					break;
				case FieldDescriptor::TYPE_BYTES:
					cout << " = " << "\"" << r->GetRepeatedStringReference(*message, fd, i, NULL) << "\"" << '\t';
					break;
				case FieldDescriptor::TYPE_UINT32:
					cout << " = " << r->GetRepeatedUInt32(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_ENUM:
					cout << " = " << "UNKNOWN_ENUM" << '\t';
					break;
				case FieldDescriptor::TYPE_SFIXED32:
					cout << " = " << r->GetRepeatedUInt32(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_SFIXED64:
					cout << " = " << r->GetRepeatedUInt64(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_SINT32:
					cout << " = " << r->GetRepeatedInt32(*message, fd, i) << '\t';
					break;
				case FieldDescriptor::TYPE_SINT64:
					cout << " = " << r->GetRepeatedInt64(*message, fd, i) << '\t';
					break;
				default:
					break;
				}
				path.pop_back();
			}
		}

		path.pop_back();
	}
}

int main()
{
	log_level(log_level_t::debug);

	vector<string> path;
	proto_parser_t(STDIN_FILENO)([&] (proto::geo_data_t const &geo_data) {
		ForEach(&geo_data, path);
		cout << endl;
	});

	return 0;
}
